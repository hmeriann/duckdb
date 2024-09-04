SELECT 
  list_aggregate(tau_pt, 'arbitrary'),
  list_aggregate(tau_eta, 'arbitrary'),
  list_aggregate(jet_pt, 'arbitrary'),
  list_aggregate(jet_eta, 'arbitrary'),
  list_aggregate(muon_pt, 'arbitrary'),
  list_aggregate(muon_eta, 'arbitrary'),
  list_aggregate(el_pt, 'arbitrary'),
  list_aggregate(el_eta, 'arbitrary'),
  list_aggregate(ph_pt, 'arbitrary'),
  list_aggregate(ph_eta, 'arbitrary')  
FROM singleMu;
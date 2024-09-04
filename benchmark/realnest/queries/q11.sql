SELECT 
  list_aggregate(tau_pt, 'fsum'),
  list_aggregate(tau_eta, 'fsum'),
  list_aggregate(jet_pt, 'fsum'),
  list_aggregate(jet_eta, 'fsum'),
  list_aggregate(muon_pt, 'fsum'),
  list_aggregate(muon_eta, 'fsum'),
  list_aggregate(el_pt, 'fsum'),
  list_aggregate(el_eta, 'fsum'),
  list_aggregate(ph_pt, 'fsum'),
  list_aggregate(ph_eta, 'fsum')  
FROM singleMu;
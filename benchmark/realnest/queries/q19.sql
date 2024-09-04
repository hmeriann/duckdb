SELECT 
  list_aggregate(tau_pt, 'list'),
  list_aggregate(tau_eta, 'list'),
  list_aggregate(jet_pt, 'list'),
  list_aggregate(jet_eta, 'list'),
  list_aggregate(muon_pt, 'list'),
  list_aggregate(muon_eta, 'list'),
  list_aggregate(el_pt, 'list'),
  list_aggregate(el_eta, 'list'),
  list_aggregate(ph_pt, 'list'),
  list_aggregate(ph_eta, 'list')  
FROM singleMu;